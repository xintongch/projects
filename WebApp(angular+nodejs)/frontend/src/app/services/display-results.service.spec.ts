import { TestBed } from '@angular/core/testing';

import { DisplayResultsService } from './display-results.service';

describe('DisplayResultsService', () => {
  let service: DisplayResultsService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(DisplayResultsService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
