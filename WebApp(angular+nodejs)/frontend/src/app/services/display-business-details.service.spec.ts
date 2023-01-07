import { TestBed } from '@angular/core/testing';

import { DisplayBusinessDetailsService } from './display-business-details.service';

describe('DisplayBusinessDetailsService', () => {
  let service: DisplayBusinessDetailsService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(DisplayBusinessDetailsService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
